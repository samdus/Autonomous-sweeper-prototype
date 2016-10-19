import { Mongo } from 'meteor/mongo';
 
export const ConsoleContainer = new Mongo.Collection('consoleContainer');

if (Meteor.isServer) {
   Meteor.publish('consoleContainer', function consoleContainerPublication() {
    console.log('PUBLISHING consoleContainer')
     return ConsoleContainer.find({}, {sort: {createdAt: -1, limit: 1}})
  });
}
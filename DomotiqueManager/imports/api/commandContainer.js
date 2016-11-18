import { Mongo } from 'meteor/mongo';
 
export const CommandContainer = new Mongo.Collection('commandContainer');

if (Meteor.isServer) {
   Meteor.publish('commandContainer', function commandContainerPublication() {
      console.log('PUBLISHING the commandContainer')



     return CommandContainer.find({}, {sort: {createdAt: -1}, limit:10 })
  });
}
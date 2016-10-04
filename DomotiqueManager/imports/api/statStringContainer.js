import { Mongo } from 'meteor/mongo';
 
export const StatContainer = new Mongo.Collection('statStringContainer');

if (Meteor.isServer) {
   Meteor.publish('statStringContainer', function statContainerPublication(collectionName) {
    console.log('PUBLISHING ' +collectionName)
    return StatContainer.find({statIdentifier: collectionName})
  });
}